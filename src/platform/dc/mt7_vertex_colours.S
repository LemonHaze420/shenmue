#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

    .section .text.dc_mt7_vertex_colours,"ax"
    .balign 32
    .global _shenmue_dc_mt7_float_vertex_colours
    .type _shenmue_dc_mt7_float_vertex_colours,@function
    .global _shenmue_dc_mt7_packed_vertex_colours
    .type _shenmue_dc_mt7_packed_vertex_colours,@function
    mov r15, r1
    fmov.s fr15, @-r15
    fmov.s fr14, @-r15
    fmov.s fr13, @-r15
    fmov.s fr12, @-r15
_shenmue_dc_mt7_float_vertex_colours:
    fmov.s fr11, @-r15
    fmov.s fr10, @-r15
    fmov.s fr9, @-r15
    fmov.s fr8, @-r15
    sts.l pr, @-r15
    mov.l .Lmt7_vertex_colours_block_0010, r0
    mov.l r0, @-r15
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0001:
    cmp/pl r0
    mov.l @r15, r9
    bt/s .Lmt7_vertex_colours_block_0002
    cmp/pz r0
    mov r0, r8
    mov r11, r0
    mov.l r5, @(r0,r10)
    add #0x4, r15
    lds.l @r15+,pr
    fmov.s @r15+, fr8
    fmov.s @r15+, fr9
    fmov.s @r15+, fr10
    fmov.s @r15+, fr11
    fmov.s @r15+, fr12
    fmov.s @r15+, fr13
    fmov.s @r15+, fr14
    rts
    fmov.s @r15+, fr15
    .word 0x0009
.Lmt7_vertex_colours_block_0002:
    mov r0, r1
    mov #-0x5, r2
    mov.l @r9+, r3
    and #0x40,r0
    shld r2, r0
    mov #-0x3, r2
    and r2, r3
    or r0, r3
    add #0x20, r4
    mov #-0x31, r2
    and r2, r3
    mov r1, r0
    mov r15, r2
    add #0x28, r2
    lds r3,fpul
    mov.l @(0x28,r2), r2
    cmp/pz r2
    mov #0x1b, r2
    bt/s .Lmt7_vertex_colours_block_0003
    and #0x3,r0
    xor #0x1,r0
.Lmt7_vertex_colours_block_0003:
    shld r2, r0
    mov #0x3, r3
    fschg
    shld r2, r3
    pref @r4
    not r3, r3
    mov.l @r9+, r2
    and r3, r2
    fsts fpul, fr0
    or r0, r2
    lds r2,fpul
    add #-0x20, r4
    fsts fpul, fr1
    add #0x10, r6
    fmov.s @r9+, fr2
    fmov.s fr2, @-r6
    add #0x20, r5
    fmov.s fr0, @-r6
    fschg
    pref @r6
    add #0x20, r6
    rotcr r12
    mov r1, r0
    bt/s .Lmt7_vertex_colours_block_0005
    shll r12
    tst #0x8,r0
    bt .Lmt7_vertex_colours_block_0004
    bsr .Lmt7_vertex_colours_block_0020
    nop
    bra .Lmt7_vertex_colours_block_0001
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0004:
    bsr .Lmt7_vertex_colours_block_0011
    nop
    bra .Lmt7_vertex_colours_block_0001
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0005:
    mov #0x0, r1
    tst #0x40,r0
    addc r1, r12
    tst #0x10,r0
    bt/s .Lmt7_vertex_colours_block_0006
    tst #0x8,r0
    bsr .Lmt7_vertex_colours_block_0029
    nop
    bra .Lmt7_vertex_colours_block_0001
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0006:
    bt .Lmt7_vertex_colours_block_0007
    bsr .Lmt7_vertex_colours_block_0078
    nop
    bra .Lmt7_vertex_colours_block_0001
    mov.l @r4+, r0
    .word 0x0009
.Lmt7_vertex_colours_block_0007:
    bra .Lmt7_vertex_colours_block_0007
    nop
    .word 0x8080
    .word 0x3b80
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0008:
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
.Lmt7_vertex_colours_block_0009:
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
.Lmt7_vertex_colours_block_0010:
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
.Lmt7_vertex_colours_block_0011:
    mov.l @r4+, r3
    add #0x20, r6
    fschg
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf .Lmt7_vertex_colours_block_0012
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0012:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0013:
    fldi0 fr3
    fldi1 fr7
    fdiv fr4, fr7
    fmov.s fr2, @-r6
    dt r3
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0018
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0014
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0014:
    add #0x20, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    add #-0x20, r4
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fmov.s @r14, fr0
    dt r3
    pref @r6
    add #0x40, r6
    fldi0 fr3
    fmov.s fr2, @-r6
    mov r6, r2
    fldi1 fr11
    fdiv fr8, fr11
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0016
    mov.l @r4+, r14
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0015
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0015:
    add #0x20, r4
    fmov.s @r14+, fr4
    pref @r4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x20, r4
    fmov.s fr8, @-r6
    add #0x40, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0013
    add #0x40, r6
    .word 0x0009
.Lmt7_vertex_colours_block_0016:
    cmp/pl r0
    mov r14, r3
    bf .Lmt7_vertex_colours_block_0017
    tst #0x80,r0
    bt .Lmt7_vertex_colours_block_0017
    mov.l @r4+, r0
    mov r6, r2
    shar r2
    mov.l @r4+, r14
    tst #0x1,r0
    add r4, r14
    bt .Lmt7_vertex_colours_block_0015
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0015
    add #-0x8, r14
.Lmt7_vertex_colours_block_0017:
    add #0x20, r5
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0019
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0018:
    cmp/pl r0
    mov r14, r3
    bf .Lmt7_vertex_colours_block_0019
    tst #0x80,r0
    bt .Lmt7_vertex_colours_block_0019
    mov.l @r4+, r0
    mov r6, r2
    shar r2
    mov.l @r4+, r14
    tst #0x1,r0
    add r4, r14
    bt .Lmt7_vertex_colours_block_0014
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0014
    add #-0x8, r14
.Lmt7_vertex_colours_block_0019:
    fmul fr7, fr6
    mov r6, r2
    shar r2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    add #-0x8, r4
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
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
.Lmt7_vertex_colours_block_0020:
    mov.l @r4+, r9
    add #0x20, r6
    fschg
    mov #0x3, r3
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf .Lmt7_vertex_colours_block_0021
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0021:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0022:
    fldi0 fr3
    fldi1 fr7
    fdiv fr4, fr7
    fmov.s fr2, @-r6
    dt r3
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0027
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0023
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0023:
    add #0x20, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    add #-0x20, r4
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fmov.s @r14, fr0
    dt r3
    pref @r6
    add #0x40, r6
    fldi0 fr3
    fmov.s fr2, @-r6
    mov r6, r2
    fldi1 fr11
    fdiv fr8, fr11
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0025
    mov.l @r4+, r14
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0024
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0024:
    add #0x20, r4
    fmov.s @r14+, fr4
    pref @r4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x20, r4
    fmov.s fr8, @-r6
    add #0x40, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0022
    add #0x40, r6
.Lmt7_vertex_colours_block_0025:
    dt r9
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0026
    shar r2
    tst #0x1,r0
    mov #0x3, r3
    bt/s .Lmt7_vertex_colours_block_0024
    add r4, r14
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0024
    add #-0x8, r14
.Lmt7_vertex_colours_block_0026:
    add #0x20, r5
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0028
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0027:
    dt r9
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0028
    shar r2
    tst #0x1,r0
    mov #0x3, r3
    bt/s .Lmt7_vertex_colours_block_0023
    add r4, r14
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0023
    add #-0x8, r14
.Lmt7_vertex_colours_block_0028:
    fmul fr7, fr6
    fmov.s fr0, @-r6
    fmul fr7, fr5
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    add #-0x8, r4
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0029:
    mov.l r10, @-r15
    mov.l r11, @-r15
    sts.l pr, @-r15
    mov.l @r4+, r3
    fschg
    fmov fr3, fr0
    fmov fr7, fr12
    fmov fr11, fr2
    fmov fr15, fr14
    fschg
    fmov fr1, fr12
    fmov fr3, fr14
.Lmt7_vertex_colours_block_0030:
    mov #0x0, r11
    bsr .Lmt7_vertex_colours_block_0066
    mov #0x0, r9
    bf .Lmt7_vertex_colours_block_0031
    bra .Lmt7_vertex_colours_block_0045
.Lmt7_vertex_colours_block_0031:
    rotcl r9
    bsr .Lmt7_vertex_colours_block_0068
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bf .Lmt7_vertex_colours_block_0032
    bra .Lmt7_vertex_colours_block_0049
.Lmt7_vertex_colours_block_0032:
    rotcl r9
    bsr .Lmt7_vertex_colours_block_0068
    add #0x1, r11
    add #0x20, r6
    mov.l @r4, r0
    fschg
    tst #0x1,r0
    mov r4, r14
    bf/s .Lmt7_vertex_colours_block_0033
    add #0x20, r4
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0033:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0034:
    fldi0 fr3
    fmov.s fr2, @-r6
    flds fr7, fpul
    fldi1 fr7
    fdiv fr4, fr7
    sts fpul,r0
    cmp/hi r0, r12
    bt .Lmt7_vertex_colours_block_0040
    dt r3
    mov.l @r4+, r0
    add #0x1, r11
    bt/s .Lmt7_vertex_colours_block_0043
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0035
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0035:
    add #0x40, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    add #-0x40, r4
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    fmov.s fr4, @-r6
    add #0x20, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    add #0x40, r6
    fldi0 fr3
    fmov.s fr2, @-r6
    flds fr11, fpul
    fldi1 fr11
    fdiv fr8, fr11
    sts fpul,r0
    cmp/hi r0, r12
    bt .Lmt7_vertex_colours_block_0039
    dt r3
    mov.l @r4+, r0
    add #0x1, r11
    bt/s .Lmt7_vertex_colours_block_0042
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0036
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0036:
    fmov.s @r14+, fr4
    add #0x40, r4
    fmov.s @r14+, fr6
    pref @r4
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x40, r4
    fmov.s fr8, @-r6
    add #0x20, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0034
    add #0x40, r6
.Lmt7_vertex_colours_block_0037:
    mov.l @r4+, r0
    cmp/pl r0
    bf/s .Lmt7_vertex_colours_block_0038
    tst #0x80,r0
    bf/s .Lmt7_vertex_colours_block_0030
    mov.l @r4+, r3
    add #-0x4, r4
.Lmt7_vertex_colours_block_0038:
    lds.l @r15+,pr
    mov #0x1, r0
    mov.l @r15+, r11
    add #-0x4, r4
    mov.l @r15+, r10
    rts
    or r0, r12
.Lmt7_vertex_colours_block_0039:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0041
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0040:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
.Lmt7_vertex_colours_block_0041:
    rotcl r9
    fschg
    mov r14, r2
    fsts fpul, fr4
    bra .Lmt7_vertex_colours_block_0059
    add #-0x18, r6
    .word 0x0009
.Lmt7_vertex_colours_block_0042:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0044
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0043:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
.Lmt7_vertex_colours_block_0044:
    fmul fr7, fr6
    mov r6, r2
    shar r2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    add #-0x8, r4
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fschg
    pref @r6
    bra .Lmt7_vertex_colours_block_0037
    add #0x20, r6
.Lmt7_vertex_colours_block_0045:
    dt r3
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0066
    dt r3
    bf/s .Lmt7_vertex_colours_block_0053
    rotcl r9
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf/s .Lmt7_vertex_colours_block_0046
    fldi1 fr7
    add #-0x18, r4
    mov.l @(0x4,r14), r14
    add r4, r14
.Lmt7_vertex_colours_block_0046:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr5
.Lmt7_vertex_colours_block_0047:
    fmov.s @r14+, fr6
    mov #0x20, r1
    mov.l @r4, r0
    add #0x20, r4
    pref @r4
    fipr fv12, fv4
    tst #0x1,r0
    add #-0x20, r4
    bf/s .Lmt7_vertex_colours_block_0048
    mov r4, r14
    mov #0x8, r1
    mov.l @(0x4,r14), r14
    add #0x8, r14
    add r4, r14
.Lmt7_vertex_colours_block_0048:
    flds fr7, fpul
    sts fpul,r0
    cmp/hi r0, r12
    add #0x1, r11
    bf/s .Lmt7_vertex_colours_block_0057
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_vertex_colours_block_0037
    add r1, r4
    fmov.s @r14+, fr4
    bra .Lmt7_vertex_colours_block_0047
    fmov.s @r14+, fr5
.Lmt7_vertex_colours_block_0049:
    bsr .Lmt7_vertex_colours_block_0070
    dt r3
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0050
    add #-0x18, r4
.Lmt7_vertex_colours_block_0050:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0051
    add #-0x18, r4
.Lmt7_vertex_colours_block_0051:
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0070
    mov #0x0, r11
    mov #0x1, r11
    clrt
    bsr .Lmt7_vertex_colours_block_0074
    mov #0x0, r0
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0052
    add #0x18, r4
.Lmt7_vertex_colours_block_0052:
    bra .Lmt7_vertex_colours_block_0064
    nop
.Lmt7_vertex_colours_block_0053:
    bsr .Lmt7_vertex_colours_block_0070
    mov #0x1, r11
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0054
    add #-0x18, r4
.Lmt7_vertex_colours_block_0054:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0055
    add #-0x18, r4
.Lmt7_vertex_colours_block_0055:
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0070
    mov #0x0, r11
    mov #0x1, r11
    clrt
    bsr .Lmt7_vertex_colours_block_0074
    mov #0x0, r0
    clrt
    bsr .Lmt7_vertex_colours_block_0072
    mov #0x1, r0
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0056
    add #0x18, r4
.Lmt7_vertex_colours_block_0056:
    bra .Lmt7_vertex_colours_block_0064
    nop
.Lmt7_vertex_colours_block_0057:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0058
    add #-0x18, r4
.Lmt7_vertex_colours_block_0058:
    bsr .Lmt7_vertex_colours_block_0066
    nop
.Lmt7_vertex_colours_block_0059:
    bsr .Lmt7_vertex_colours_block_0070
    nop
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0060
    add #-0x18, r4
.Lmt7_vertex_colours_block_0060:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0061
    add #-0x18, r4
.Lmt7_vertex_colours_block_0061:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0062
    add #-0x18, r4
.Lmt7_vertex_colours_block_0062:
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0070
    add #-0x2, r11
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0070
    add #0x1, r11
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0063
    add #0x18, r4
.Lmt7_vertex_colours_block_0063:
    add #0x1, r11
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0065
    mov r9, r0
    bsr .Lmt7_vertex_colours_block_0071
    nop
    bra .Lmt7_vertex_colours_block_0065
    mov r9, r0
.Lmt7_vertex_colours_block_0064:
    bsr .Lmt7_vertex_colours_block_0066
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0070
    rotcl r9
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0065
    mov r9, r0
    bsr .Lmt7_vertex_colours_block_0071
    nop
    mov r9, r0
.Lmt7_vertex_colours_block_0065:
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
    .word 0xb0ed
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0c9
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb0e5
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf0a
    .word 0x0009
    .word 0x0008
    .word 0xb0bf
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb0db
    .word 0x70ff
    .word 0x4310
    .word 0xb0b8
    .word 0x60b3
    .word 0x8bc6
    .word 0xaefd
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb0d1
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf30
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0c9
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0a5
    .word 0x70ff
    .word 0x4310
    .word 0xb0a2
    .word 0x60b3
    .word 0x8901
    .word 0xae8a
    .word 0x7b01
    .word 0xaee5
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb0b9
    .word 0x70ff
    .word 0x8ba7
    .word 0xaede
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x890d
    .word 0x60b3
    .word 0x0008
    .word 0xb0af
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0ab
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0a7
    .word 0x70ff
    .word 0xa008
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0a1
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb09d
    .word 0x70ff
    .word 0x4310
    .word 0xb07a
    .word 0x60b3
    .word 0x8b88
    .word 0xaebf
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0066:
    mov.l @r4, r0
    fschg
    tst #0x1,r0
    mov r4, r14
    bf/s .Lmt7_vertex_colours_block_0067
    add #0x20, r4
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0067:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fldi0 fr3
    ftrv xmtrx, fv4
    add #0x40, r4
    mov r14, r2
    pref @r4
    add #-0x40, r4
    flds fr7, fpul
    fldi1 fr7
    fdiv fr4, fr7
    sts fpul,r0
    fschg
    fsts fpul, fr4
    rts
    cmp/hi r0, r12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0068:
    fschg
    add #0x20, r6
    fmov.s @r2, fr0
    fmul fr7, fr6
    fmov.s fr2, @-r6
    fmul fr7, fr5
    add #0x20, r5
    fmov.s fr0, @-r6
    mov r6, r2
    fmov.s fr6, @-r6
    dt r3
    fmov.s fr4, @-r6
    bf .Lmt7_vertex_colours_block_0069
    shar r2
    sett
.Lmt7_vertex_colours_block_0069:
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
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
.Lmt7_vertex_colours_block_0070:
    mov #0x5, r10
    mov r11, r0
    and #0x3,r0
    fschg
    shld r10, r0
    mov.l .Lmt7_vertex_colours_block_0077, r10
    fmov.s @r2, fr0
    add r0, r10
    fmov.s fr2, @-r10
    fmov.s fr0, @-r10
    fmov.s fr6, @-r10
    fmov.s fr4, @-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0071:
    mov #0x5, r1
    mov.l .Lmt7_vertex_colours_block_0077, r10
    mov r11, r0
    and #0x3,r0
    mov r10, r2
    shld r1, r0
    fschg
    add r0, r10
    add #-0x8, r10
    mov r11, r0
    fmov.s @r10, fr2
    add #-0x1, r0
    mov r2, r10
    and #0x3,r0
    shld r1, r0
    add r0, r2
    mov r11, r0
    fmov.s fr2, @-r2
    add #-0x2, r0
    and #0x3,r0
    shld r1, r0
    add r0, r10
    fmov.s fr2, @-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0072:
    and #0x3,r0
    mov #0x5, r2
    mov.l .Lmt7_vertex_colours_block_0076, r10
    shld r2, r0
    fschg
    add r0, r10
    fmov.s @r10+, fr0
    fmov.s @r10+, fr2
    add #0x20, r6
    fmov.s @r10+, fr4
    fmul fr3, fr2
    fmov.s @r10+, fr6
    fmul fr3, fr1
    fmov.s fr6, @-r6
    mov r6, r2
    fmov.s fr4, @-r6
    add #0x20, r5
    fmov.s fr2, @-r6
    bf/s .Lmt7_vertex_colours_block_0073
    fmov.s fr0, @-r6
    shar r2
    sett
.Lmt7_vertex_colours_block_0073:
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0074:
    and #0x3,r0
    shll2 r0
    mov.l .Lmt7_vertex_colours_block_0076, r10
    shll2 r0
    add r0, r0
    mov r10, r2
    add r0, r10
    mov r11, r0
    and #0x3,r0
    fmov.s @r10, fr2
    shll2 r0
    shll2 r0
    add r0, r0
    add r0, r2
    fmov.s @r2, fr1
    lds r12,fpul
    fsts fpul, fr0
    fsub fr2, fr0
    add #0x18, r10
    fsub fr2, fr1
    add #0x18, r2
    fdiv fr1, fr0
    lds r7,fpul
    fsts fpul, fr1
    fschg
    fmov.s @r2, fr4
    add #-0x8, r2
    fmov.s @r10, fr2
    add #-0x8, r10
    flds fr4, fpul
    sts fpul,r0
    shlr16 r0
    mov r0, r1
    shlr8 r0
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr8
    flds fr2, fpul
    extu.b r1, r1
    sts fpul,r0
    shlr16 r0
    lds r1,fpul
    mov r0, r1
    shlr8 r0
    float fpul, fr10
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr9
    fsub fr9, fr8
    fmac fr0, fr8, fr9
    extu.b r1, r1
    ftrc fr9, fpul
    sts fpul,r8
    shll8 r8
    lds r1,fpul
    float fpul, fr11
    flds fr4, fpul
    fsub fr11, fr10
    sts fpul,r0
    fmac fr0, fr10, fr11
    shlr8 r0
    ftrc fr11, fpul
    extu.b r0, r0
    sts fpul,r1
    or r1, r8
    lds r0,fpul
    float fpul, fr8
    flds fr2, fpul
    shll8 r8
    sts fpul,r0
    shlr8 r0
    flds fr4, fpul
    extu.b r0, r0
    sts fpul,r1
    extu.b r1, r1
    lds r0,fpul
    float fpul, fr9
    lds r1,fpul
    float fpul, fr10
    fsub fr9, fr8
    flds fr2, fpul
    fmac fr0, fr8, fr9
    sts fpul,r1
    ftrc fr9, fpul
    extu.b r1, r1
    sts fpul,r0
    or r0, r8
    lds r1,fpul
    float fpul, fr11
    fsub fr11, fr10
    shll8 r8
    fmac fr0, fr10, fr11
    ftrc fr11, fpul
    sts fpul,r1
    or r1, r8
    fldi0 fr3
    add #0x20, r6
    lds r8,fpul
    fsts fpul, fr2
    fmov.s fr2, @-r6
    fmov.s @r2, fr4
    add #-0x8, r2
    fmov.s @r10, fr2
    add #-0x8, r10
    fsub fr2, fr4
    fsub fr3, fr5
    fmac fr0, fr4, fr2
    fmac fr0, fr5, fr3
    fmov.s fr2, @-r6
    fschg
    fmov.s fr1, @-r6
    fmov.s @r2, fr5
    add #-0x4, r2
    fmov.s @r10, fr3
    add #-0x4, r10
    fmov.s @r2, fr4
    fmov.s @r10, fr2
    fsub fr3, fr5
    fsub fr2, fr4
    mov #-0x10, r2
    fmac fr0, fr5, fr3
    shll16 r2
    fmac fr0, fr4, fr2
    shll8 r2
    fmul fr1, fr3
    add #0x20, r5
    fmul fr1, fr2
    fmov.s fr3, @-r6
    fmov.s fr2, @-r6
    bt .Lmt7_vertex_colours_block_0075
    mov r6, r2
.Lmt7_vertex_colours_block_0075:
    mov.l r2, @-r6
    pref @r6
    rts
    add #0x20, r6
    .word 0x0009
.Lmt7_vertex_colours_block_0076:
    .long .Lmt7_vertex_colours_block_0008
.Lmt7_vertex_colours_block_0077:
    .long .Lmt7_vertex_colours_block_0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0078:
    mov.l r10, @-r15
    mov.l r11, @-r15
    sts.l pr, @-r15
    mov.l @r4+, r3
.Lmt7_vertex_colours_block_0079:
    bsr .Lmt7_vertex_colours_block_0087
    mov #0x0, r9
    mov r9, r0
    cmp/eq #0x0,r0
    bt .Lmt7_vertex_colours_block_0082
    cmp/eq #0x7,r0
    bt .Lmt7_vertex_colours_block_0083
    bsr .Lmt7_vertex_colours_block_0066
    mov #0x0, r11
    bsr .Lmt7_vertex_colours_block_0070
    nop
    bsr .Lmt7_vertex_colours_block_0066
    mov #0x1, r11
    bsr .Lmt7_vertex_colours_block_0070
    nop
    bsr .Lmt7_vertex_colours_block_0066
    mov #0x2, r11
    bsr .Lmt7_vertex_colours_block_0070
    nop
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0080
    shll2 r9
    bsr .Lmt7_vertex_colours_block_0071
    nop
.Lmt7_vertex_colours_block_0080:
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
.Lmt7_vertex_colours_block_0081:
    dt r3
    bf .Lmt7_vertex_colours_block_0079
    mov #0x1, r0
    lds.l @r15+,pr
    or r0, r12
    mov.l @r15+, r11
    rts
    mov.l @r15+, r10
.Lmt7_vertex_colours_block_0082:
    mov r3, r9
    bsr .Lmt7_vertex_colours_block_0066
    mov #0x3, r3
    bsr .Lmt7_vertex_colours_block_0068
    nop
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0068
    nop
    bsr .Lmt7_vertex_colours_block_0066
    nop
    bsr .Lmt7_vertex_colours_block_0068
    nop
    bra .Lmt7_vertex_colours_block_0081
    mov r9, r3
.Lmt7_vertex_colours_block_0083:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0084
    add #0x18, r4
.Lmt7_vertex_colours_block_0084:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0085
    add #0x18, r4
.Lmt7_vertex_colours_block_0085:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0086
    add #0x18, r4
.Lmt7_vertex_colours_block_0086:
    bra .Lmt7_vertex_colours_block_0081
    nop
    .word 0x0008
    .word 0xbef6
    .word 0xe000
    .word 0x0008
    .word 0xbef3
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf0f
    .word 0xe000
    .word 0x0018
    .word 0xbf0c
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbee7
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf03
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbedf
    .word 0xe002
    .word 0x0018
    .word 0xbefc
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbed7
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbef3
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbeef
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbee9
    .word 0xe000
    .word 0x0008
    .word 0xbec6
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbee2
    .word 0xe000
    .word 0x0018
    .word 0xbebf
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbed9
    .word 0xe000
    .word 0x0008
    .word 0xbeb6
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbed2
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbecc
    .word 0xe000
    .word 0x0008
    .word 0xbec9
    .word 0xe001
    .word 0x0018
    .word 0xbea6
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0087:
    mov r4, r1
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0088
    fmov.s @r4+, fr4
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr4
    fmov.s @r0+, fr5
    fmov.s @r0+, fr6
    bra .Lmt7_vertex_colours_block_0089
    fldi1 fr7
.Lmt7_vertex_colours_block_0088:
    fmov.s @r4+, fr5
    fmov.s @r4, fr6
    fldi1 fr7
    add #0x18, r4
.Lmt7_vertex_colours_block_0089:
    pref @r4
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    ftrv xmtrx, fv4
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0090
    fmov.s @r4+, fr0
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr0
    fmov.s @r0+, fr1
    fmov.s @r0+, fr2
    bra .Lmt7_vertex_colours_block_0091
    fldi1 fr3
.Lmt7_vertex_colours_block_0090:
    fmov.s @r4+, fr1
    fmov.s @r4, fr2
    fldi1 fr3
    add #0x18, r4
.Lmt7_vertex_colours_block_0091:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    ftrv xmtrx, fv0
    flds fr7, fpul
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0092
    fmov.s @r4+, fr4
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr4
    fmov.s @r0+, fr5
    fmov.s @r0+, fr6
    bra .Lmt7_vertex_colours_block_0093
    fldi1 fr7
.Lmt7_vertex_colours_block_0092:
    fmov.s @r4+, fr5
    fmov.s @r4, fr6
    fldi1 fr7
.Lmt7_vertex_colours_block_0093:
    mov r1, r4
    sts fpul,r0
    ftrv xmtrx, fv4
    cmp/hi r0, r12
    flds fr3, fpul
    rotcl r9
    sts fpul,r0
    cmp/hi r0, r12
    flds fr7, fpul
    rotcl r9
    sts fpul,r0
    cmp/hi r0, r12
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
    .word 0x0000
    mov r15, r1
    fmov.s fr15, @-r15
    fmov.s fr14, @-r15
    fmov.s fr13, @-r15
    fmov.s fr12, @-r15
_shenmue_dc_mt7_packed_vertex_colours:
    fmov.s fr11, @-r15
    fmov.s fr10, @-r15
    fmov.s fr9, @-r15
    fmov.s fr8, @-r15
    sts.l pr, @-r15
    mov.l .Lmt7_vertex_colours_block_0104, r0
    mov.l r0, @-r15
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0095:
    cmp/pl r0
    mov.l @r15, r9
    bt/s .Lmt7_vertex_colours_block_0096
    cmp/pz r0
    mov r0, r8
    mov r11, r0
    mov.l r5, @(r0,r10)
    add #0x4, r15
    lds.l @r15+,pr
    fmov.s @r15+, fr8
    fmov.s @r15+, fr9
    fmov.s @r15+, fr10
    fmov.s @r15+, fr11
    fmov.s @r15+, fr12
    fmov.s @r15+, fr13
    fmov.s @r15+, fr14
    rts
    fmov.s @r15+, fr15
    .word 0x0009
.Lmt7_vertex_colours_block_0096:
    mov r0, r1
    mov #-0x5, r2
    mov.l @r9+, r3
    and #0x40,r0
    shld r2, r0
    mov #-0x3, r2
    and r2, r3
    or r0, r3
    add #0x20, r4
    mov #-0x31, r2
    and r2, r3
    mov r1, r0
    mov r15, r2
    add #0x28, r2
    lds r3,fpul
    mov.l @(0x28,r2), r2
    cmp/pz r2
    mov #0x1b, r2
    bt/s .Lmt7_vertex_colours_block_0097
    and #0x3,r0
    xor #0x1,r0
.Lmt7_vertex_colours_block_0097:
    shld r2, r0
    mov #0x3, r3
    fschg
    shld r2, r3
    pref @r4
    not r3, r3
    mov.l @r9+, r2
    and r3, r2
    fsts fpul, fr0
    or r0, r2
    lds r2,fpul
    add #-0x20, r4
    fsts fpul, fr1
    add #0x10, r6
    fmov.s @r9+, fr2
    fmov.s fr2, @-r6
    add #0x20, r5
    fmov.s fr0, @-r6
    fschg
    pref @r6
    add #0x20, r6
    rotcr r12
    mov r1, r0
    bt/s .Lmt7_vertex_colours_block_0099
    shll r12
    tst #0x8,r0
    bt .Lmt7_vertex_colours_block_0098
    bsr .Lmt7_vertex_colours_block_0114
    nop
    bra .Lmt7_vertex_colours_block_0095
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0098:
    bsr .Lmt7_vertex_colours_block_0105
    nop
    bra .Lmt7_vertex_colours_block_0095
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0099:
    mov #0x0, r1
    tst #0x40,r0
    addc r1, r12
    tst #0x10,r0
    bt/s .Lmt7_vertex_colours_block_0100
    tst #0x8,r0
    bsr .Lmt7_vertex_colours_block_0123
    nop
    bra .Lmt7_vertex_colours_block_0095
    mov.l @r4+, r0
.Lmt7_vertex_colours_block_0100:
    bt .Lmt7_vertex_colours_block_0101
    bsr .Lmt7_vertex_colours_block_0172
    nop
    bra .Lmt7_vertex_colours_block_0095
    mov.l @r4+, r0
    .word 0x0009
.Lmt7_vertex_colours_block_0101:
    bra .Lmt7_vertex_colours_block_0101
    nop
    .word 0x8080
    .word 0x3b80
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0102:
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
.Lmt7_vertex_colours_block_0103:
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
.Lmt7_vertex_colours_block_0104:
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
.Lmt7_vertex_colours_block_0105:
    mov.l @r4+, r3
    add #0x20, r6
    fschg
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf .Lmt7_vertex_colours_block_0106
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0106:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0107:
    fldi1 fr7
    fdiv fr4, fr7
    fmov.s fr2, @-r6
    dt r3
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0112
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0108
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0108:
    add #0x20, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    add #-0x20, r4
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fmov.s @r14, fr0
    dt r3
    pref @r6
    add #0x40, r6
    fmov.s fr2, @-r6
    mov r6, r2
    fldi1 fr11
    fdiv fr8, fr11
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0110
    mov.l @r4+, r14
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0109
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0109:
    add #0x20, r4
    fmov.s @r14+, fr4
    pref @r4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x20, r4
    fmov.s fr8, @-r6
    add #0x40, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0107
    add #0x40, r6
    .word 0x0009
.Lmt7_vertex_colours_block_0110:
    cmp/pl r0
    mov r14, r3
    bf .Lmt7_vertex_colours_block_0111
    tst #0x80,r0
    bt .Lmt7_vertex_colours_block_0111
    mov.l @r4+, r0
    mov r6, r2
    shar r2
    mov.l @r4+, r14
    tst #0x1,r0
    add r4, r14
    bt .Lmt7_vertex_colours_block_0109
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0109
    add #-0x8, r14
.Lmt7_vertex_colours_block_0111:
    add #0x20, r5
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0113
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0112:
    cmp/pl r0
    mov r14, r3
    bf .Lmt7_vertex_colours_block_0113
    tst #0x80,r0
    bt .Lmt7_vertex_colours_block_0113
    mov.l @r4+, r0
    mov r6, r2
    shar r2
    mov.l @r4+, r14
    tst #0x1,r0
    add r4, r14
    bt .Lmt7_vertex_colours_block_0108
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0108
    add #-0x8, r14
.Lmt7_vertex_colours_block_0113:
    fmul fr7, fr6
    mov r6, r2
    shar r2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    add #-0x8, r4
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
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
.Lmt7_vertex_colours_block_0114:
    mov.l @r4+, r9
    add #0x20, r6
    fschg
    mov #0x3, r3
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf .Lmt7_vertex_colours_block_0115
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0115:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0116:
    fldi1 fr7
    fdiv fr4, fr7
    fmov.s fr2, @-r6
    dt r3
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0121
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0117
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0117:
    add #0x20, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    add #-0x20, r4
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fmov.s @r14, fr0
    dt r3
    pref @r6
    add #0x40, r6
    fmov.s fr2, @-r6
    mov r6, r2
    fldi1 fr11
    fdiv fr8, fr11
    mov.l @r4+, r0
    bt/s .Lmt7_vertex_colours_block_0119
    mov.l @r4+, r14
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0118
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0118:
    add #0x20, r4
    fmov.s @r14+, fr4
    pref @r4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x20, r4
    fmov.s fr8, @-r6
    add #0x40, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0116
    add #0x40, r6
.Lmt7_vertex_colours_block_0119:
    dt r9
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0120
    shar r2
    tst #0x1,r0
    mov #0x3, r3
    bt/s .Lmt7_vertex_colours_block_0118
    add r4, r14
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0118
    add #-0x8, r14
.Lmt7_vertex_colours_block_0120:
    add #0x20, r5
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0122
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0121:
    dt r9
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0122
    shar r2
    tst #0x1,r0
    mov #0x3, r3
    bt/s .Lmt7_vertex_colours_block_0117
    add r4, r14
    mov r4, r14
    add #0x18, r4
    bra .Lmt7_vertex_colours_block_0117
    add #-0x8, r14
.Lmt7_vertex_colours_block_0122:
    fmul fr7, fr6
    fmov.s fr0, @-r6
    fmul fr7, fr5
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    add #-0x8, r4
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0123:
    mov.l r10, @-r15
    mov.l r11, @-r15
    sts.l pr, @-r15
    mov.l @r4+, r3
    fschg
    fmov fr3, fr0
    fmov fr7, fr12
    fmov fr11, fr2
    fmov fr15, fr14
    fschg
    fmov fr1, fr12
    fmov fr3, fr14
.Lmt7_vertex_colours_block_0124:
    mov #0x0, r11
    bsr .Lmt7_vertex_colours_block_0160
    mov #0x0, r9
    bf .Lmt7_vertex_colours_block_0125
    bra .Lmt7_vertex_colours_block_0139
.Lmt7_vertex_colours_block_0125:
    rotcl r9
    bsr .Lmt7_vertex_colours_block_0162
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bf .Lmt7_vertex_colours_block_0126
    bra .Lmt7_vertex_colours_block_0143
.Lmt7_vertex_colours_block_0126:
    rotcl r9
    bsr .Lmt7_vertex_colours_block_0162
    add #0x1, r11
    add #0x20, r6
    mov.l @r4, r0
    fschg
    tst #0x1,r0
    mov r4, r14
    bf/s .Lmt7_vertex_colours_block_0127
    add #0x20, r4
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0127:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    fmov.s @r14, fr0
    ftrv xmtrx, fv4
.Lmt7_vertex_colours_block_0128:
    fmov.s fr2, @-r6
    flds fr7, fpul
    fldi1 fr7
    fdiv fr4, fr7
    sts fpul,r0
    cmp/hi r0, r12
    bt .Lmt7_vertex_colours_block_0134
    dt r3
    mov.l @r4+, r0
    add #0x1, r11
    bt/s .Lmt7_vertex_colours_block_0137
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0129
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0129:
    add #0x40, r4
    fmov.s @r14+, fr8
    pref @r4
    fmov.s @r14+, fr10
    add #-0x40, r4
    fldi1 fr11
    fmul fr7, fr6
    fmov.s @r14+, fr2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    ftrv xmtrx, fv8
    fmov.s fr6, @-r6
    fmov.s fr4, @-r6
    add #0x20, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    add #0x40, r6
    fmov.s fr2, @-r6
    flds fr11, fpul
    fldi1 fr11
    fdiv fr8, fr11
    sts fpul,r0
    cmp/hi r0, r12
    bt .Lmt7_vertex_colours_block_0133
    dt r3
    mov.l @r4+, r0
    add #0x1, r11
    bt/s .Lmt7_vertex_colours_block_0136
    mov.l @r4+, r14
    tst #0x1,r0
    mov r6, r2
    bt/s .Lmt7_vertex_colours_block_0130
    add r4, r14
    mov r4, r14
    add #0x18, r4
    add #-0x8, r14
.Lmt7_vertex_colours_block_0130:
    fmov.s @r14+, fr4
    add #0x40, r4
    fmov.s @r14+, fr6
    pref @r4
    fldi1 fr7
    fmul fr11, fr10
    fmov.s @r14+, fr2
    fmul fr11, fr9
    fmov.s fr0, @-r6
    ftrv xmtrx, fv4
    fmov.s fr10, @-r6
    add #-0x40, r4
    fmov.s fr8, @-r6
    add #0x20, r5
    mov.l r2, @r6
    fmov.s @r14, fr0
    pref @r6
    bra .Lmt7_vertex_colours_block_0128
    add #0x40, r6
.Lmt7_vertex_colours_block_0131:
    mov.l @r4+, r0
    cmp/pl r0
    bf/s .Lmt7_vertex_colours_block_0132
    tst #0x80,r0
    bf/s .Lmt7_vertex_colours_block_0124
    mov.l @r4+, r3
    add #-0x4, r4
.Lmt7_vertex_colours_block_0132:
    lds.l @r15+,pr
    mov #0x1, r0
    mov.l @r15+, r11
    add #-0x4, r4
    mov.l @r15+, r10
    rts
    or r0, r12
.Lmt7_vertex_colours_block_0133:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0135
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0134:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
.Lmt7_vertex_colours_block_0135:
    rotcl r9
    fschg
    mov r14, r2
    fsts fpul, fr4
    bra .Lmt7_vertex_colours_block_0153
    add #-0x18, r6
    .word 0x0009
.Lmt7_vertex_colours_block_0136:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    fmov fr8, fr4
    bra .Lmt7_vertex_colours_block_0138
    fmov fr10, fr6
.Lmt7_vertex_colours_block_0137:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
.Lmt7_vertex_colours_block_0138:
    fmul fr7, fr6
    mov r6, r2
    shar r2
    fmul fr7, fr5
    fmov.s fr0, @-r6
    add #-0x8, r4
    fmov.s fr6, @-r6
    add #0x20, r5
    fmov.s fr4, @-r6
    mov.l r2, @r6
    fschg
    pref @r6
    bra .Lmt7_vertex_colours_block_0131
    add #0x20, r6
.Lmt7_vertex_colours_block_0139:
    dt r3
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0160
    dt r3
    bf/s .Lmt7_vertex_colours_block_0147
    rotcl r9
    mov.l @r4, r0
    mov r4, r14
    tst #0x1,r0
    add #0x20, r4
    bf/s .Lmt7_vertex_colours_block_0140
    fldi1 fr7
    add #-0x18, r4
    mov.l @(0x4,r14), r14
    add r4, r14
.Lmt7_vertex_colours_block_0140:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr5
.Lmt7_vertex_colours_block_0141:
    fmov.s @r14+, fr6
    mov #0x20, r1
    mov.l @r4, r0
    add #0x20, r4
    pref @r4
    fipr fv12, fv4
    tst #0x1,r0
    add #-0x20, r4
    bf/s .Lmt7_vertex_colours_block_0142
    mov r4, r14
    mov #0x8, r1
    mov.l @(0x4,r14), r14
    add #0x8, r14
    add r4, r14
.Lmt7_vertex_colours_block_0142:
    flds fr7, fpul
    sts fpul,r0
    cmp/hi r0, r12
    add #0x1, r11
    bf/s .Lmt7_vertex_colours_block_0151
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_vertex_colours_block_0131
    add r1, r4
    fmov.s @r14+, fr4
    bra .Lmt7_vertex_colours_block_0141
    fmov.s @r14+, fr5
.Lmt7_vertex_colours_block_0143:
    bsr .Lmt7_vertex_colours_block_0164
    dt r3
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0144
    add #-0x18, r4
.Lmt7_vertex_colours_block_0144:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0145
    add #-0x18, r4
.Lmt7_vertex_colours_block_0145:
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0164
    mov #0x0, r11
    mov #0x1, r11
    clrt
    bsr .Lmt7_vertex_colours_block_0168
    mov #0x0, r0
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0146
    add #0x18, r4
.Lmt7_vertex_colours_block_0146:
    bra .Lmt7_vertex_colours_block_0158
    nop
.Lmt7_vertex_colours_block_0147:
    bsr .Lmt7_vertex_colours_block_0164
    mov #0x1, r11
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0148
    add #-0x18, r4
.Lmt7_vertex_colours_block_0148:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0149
    add #-0x18, r4
.Lmt7_vertex_colours_block_0149:
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0164
    mov #0x0, r11
    mov #0x1, r11
    clrt
    bsr .Lmt7_vertex_colours_block_0168
    mov #0x0, r0
    clrt
    bsr .Lmt7_vertex_colours_block_0166
    mov #0x1, r0
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0150
    add #0x18, r4
.Lmt7_vertex_colours_block_0150:
    bra .Lmt7_vertex_colours_block_0158
    nop
.Lmt7_vertex_colours_block_0151:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0152
    add #-0x18, r4
.Lmt7_vertex_colours_block_0152:
    bsr .Lmt7_vertex_colours_block_0160
    nop
.Lmt7_vertex_colours_block_0153:
    bsr .Lmt7_vertex_colours_block_0164
    nop
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0154
    add #-0x18, r4
.Lmt7_vertex_colours_block_0154:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0155
    add #-0x18, r4
.Lmt7_vertex_colours_block_0155:
    add #-0x8, r4
    mov.l @(0x4,r4), r0
    tst #0x1,r0
    bt .Lmt7_vertex_colours_block_0156
    add #-0x18, r4
.Lmt7_vertex_colours_block_0156:
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0164
    add #-0x2, r11
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0164
    add #0x1, r11
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0157
    add #0x18, r4
.Lmt7_vertex_colours_block_0157:
    add #0x1, r11
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0159
    mov r9, r0
    bsr .Lmt7_vertex_colours_block_0165
    nop
    bra .Lmt7_vertex_colours_block_0159
    mov r9, r0
.Lmt7_vertex_colours_block_0158:
    bsr .Lmt7_vertex_colours_block_0160
    add #0x1, r11
    bsr .Lmt7_vertex_colours_block_0164
    rotcl r9
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0159
    mov r9, r0
    bsr .Lmt7_vertex_colours_block_0165
    nop
    mov r9, r0
.Lmt7_vertex_colours_block_0159:
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
    .word 0xb0ef
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0cb
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb0e7
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf0a
    .word 0x0009
    .word 0x0008
    .word 0xb0c1
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb0dd
    .word 0x70ff
    .word 0x4310
    .word 0xb0ba
    .word 0x60b3
    .word 0x8bc6
    .word 0xaefd
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb0d3
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf30
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0cb
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0a7
    .word 0x70ff
    .word 0x4310
    .word 0xb0a4
    .word 0x60b3
    .word 0x8901
    .word 0xae8c
    .word 0x7b01
    .word 0xaee5
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb0bb
    .word 0x70ff
    .word 0x8ba7
    .word 0xaede
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x890d
    .word 0x60b3
    .word 0x0008
    .word 0xb0b1
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0ad
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0a9
    .word 0x70ff
    .word 0xa008
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0a3
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb09f
    .word 0x70ff
    .word 0x4310
    .word 0xb07c
    .word 0x60b3
    .word 0x8b88
    .word 0xaebf
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
.Lmt7_vertex_colours_block_0160:
    mov.l @r4, r0
    fschg
    tst #0x1,r0
    mov r4, r14
    bf/s .Lmt7_vertex_colours_block_0161
    add #0x20, r4
    mov.l @(0x4,r14), r14
    add #-0x18, r4
    add r4, r14
.Lmt7_vertex_colours_block_0161:
    fmov.s @r14+, fr4
    fmov.s @r14+, fr6
    fldi1 fr7
    fmov.s @r14+, fr2
    ftrv xmtrx, fv4
    add #0x40, r4
    mov r14, r2
    pref @r4
    add #-0x40, r4
    flds fr7, fpul
    fldi1 fr7
    fdiv fr4, fr7
    sts fpul,r0
    fschg
    fsts fpul, fr4
    rts
    cmp/hi r0, r12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0162:
    fschg
    add #0x20, r6
    fmov.s @r2, fr0
    fmul fr7, fr6
    fmov.s fr2, @-r6
    fmul fr7, fr5
    add #0x20, r5
    fmov.s fr0, @-r6
    mov r6, r2
    fmov.s fr6, @-r6
    dt r3
    fmov.s fr4, @-r6
    bf .Lmt7_vertex_colours_block_0163
    shar r2
    sett
.Lmt7_vertex_colours_block_0163:
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
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
.Lmt7_vertex_colours_block_0164:
    mov #0x5, r10
    mov r11, r0
    and #0x3,r0
    fschg
    shld r10, r0
    mov.l .Lmt7_vertex_colours_block_0171, r10
    fmov.s @r2, fr0
    add r0, r10
    fmov.s fr2, @-r10
    fmov.s fr0, @-r10
    fmov.s fr6, @-r10
    fmov.s fr4, @-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0165:
    mov #0x5, r1
    mov.l .Lmt7_vertex_colours_block_0171, r10
    mov r11, r0
    and #0x3,r0
    mov r10, r2
    shld r1, r0
    fschg
    add r0, r10
    add #-0x8, r10
    mov r11, r0
    fmov.s @r10, fr2
    add #-0x1, r0
    mov r2, r10
    and #0x3,r0
    shld r1, r0
    add r0, r2
    mov r11, r0
    fmov.s fr2, @-r2
    add #-0x2, r0
    and #0x3,r0
    shld r1, r0
    add r0, r10
    fmov.s fr2, @-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0166:
    and #0x3,r0
    mov #0x5, r2
    mov.l .Lmt7_vertex_colours_block_0170, r10
    shld r2, r0
    fschg
    add r0, r10
    fmov.s @r10+, fr0
    fmov.s @r10+, fr2
    add #0x20, r6
    fmov.s @r10+, fr4
    fmul fr3, fr2
    fmov.s @r10+, fr6
    fmul fr3, fr1
    fmov.s fr6, @-r6
    mov r6, r2
    fmov.s fr4, @-r6
    add #0x20, r5
    fmov.s fr2, @-r6
    bf/s .Lmt7_vertex_colours_block_0167
    fmov.s fr0, @-r6
    shar r2
    sett
.Lmt7_vertex_colours_block_0167:
    mov.l r2, @r6
    fschg
    pref @r6
    rts
    add #0x20, r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0168:
    and #0x3,r0
    shll2 r0
    mov.l .Lmt7_vertex_colours_block_0170, r10
    shll2 r0
    add r0, r0
    mov r10, r2
    add r0, r10
    mov r11, r0
    and #0x3,r0
    fmov.s @r10, fr2
    shll2 r0
    shll2 r0
    add r0, r0
    add r0, r2
    fmov.s @r2, fr1
    lds r12,fpul
    fsts fpul, fr0
    fsub fr2, fr0
    add #0x18, r10
    fsub fr2, fr1
    add #0x18, r2
    fdiv fr1, fr0
    lds r7,fpul
    fsts fpul, fr1
    fschg
    fmov.s @r2, fr4
    add #-0x8, r2
    fmov.s @r10, fr2
    add #-0x8, r10
    flds fr4, fpul
    sts fpul,r0
    shlr16 r0
    mov r0, r1
    shlr8 r0
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr8
    flds fr2, fpul
    extu.b r1, r1
    sts fpul,r0
    shlr16 r0
    lds r1,fpul
    mov r0, r1
    shlr8 r0
    float fpul, fr10
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr9
    fsub fr9, fr8
    fmac fr0, fr8, fr9
    extu.b r1, r1
    ftrc fr9, fpul
    sts fpul,r8
    shll8 r8
    lds r1,fpul
    float fpul, fr11
    flds fr4, fpul
    fsub fr11, fr10
    sts fpul,r0
    fmac fr0, fr10, fr11
    shlr8 r0
    ftrc fr11, fpul
    extu.b r0, r0
    sts fpul,r1
    or r1, r8
    lds r0,fpul
    float fpul, fr8
    flds fr2, fpul
    shll8 r8
    sts fpul,r0
    shlr8 r0
    flds fr4, fpul
    extu.b r0, r0
    sts fpul,r1
    extu.b r1, r1
    lds r0,fpul
    float fpul, fr9
    lds r1,fpul
    float fpul, fr10
    fsub fr9, fr8
    flds fr2, fpul
    fmac fr0, fr8, fr9
    sts fpul,r1
    ftrc fr9, fpul
    extu.b r1, r1
    sts fpul,r0
    or r0, r8
    lds r1,fpul
    float fpul, fr11
    fsub fr11, fr10
    shll8 r8
    fmac fr0, fr10, fr11
    ftrc fr11, fpul
    sts fpul,r1
    or r1, r8
    flds fr5, fpul
    add #0x20, r6
    sts fpul,r0
    shlr16 r0
    lds r8,fpul
    mov r0, r1
    shlr8 r0
    fsts fpul, fr2
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr8
    flds fr3, fpul
    extu.b r1, r1
    sts fpul,r0
    shlr16 r0
    lds r1,fpul
    mov r0, r1
    shlr8 r0
    float fpul, fr10
    extu.b r0, r0
    lds r0,fpul
    float fpul, fr9
    fsub fr9, fr8
    fmac fr0, fr8, fr9
    extu.b r1, r1
    ftrc fr9, fpul
    sts fpul,r8
    shll8 r8
    lds r1,fpul
    float fpul, fr11
    flds fr5, fpul
    fsub fr11, fr10
    sts fpul,r0
    fmac fr0, fr10, fr11
    shlr8 r0
    ftrc fr11, fpul
    extu.b r0, r0
    sts fpul,r1
    or r1, r8
    lds r0,fpul
    float fpul, fr8
    flds fr3, fpul
    shll8 r8
    sts fpul,r0
    shlr8 r0
    flds fr5, fpul
    extu.b r0, r0
    sts fpul,r1
    extu.b r1, r1
    lds r0,fpul
    float fpul, fr9
    lds r1,fpul
    float fpul, fr10
    fsub fr9, fr8
    flds fr3, fpul
    fmac fr0, fr8, fr9
    sts fpul,r1
    ftrc fr9, fpul
    extu.b r1, r1
    sts fpul,r0
    or r0, r8
    lds r1,fpul
    float fpul, fr11
    fsub fr11, fr10
    shll8 r8
    fmac fr0, fr10, fr11
    ftrc fr11, fpul
    sts fpul,r1
    or r1, r8
    lds r8,fpul
    fsts fpul, fr3
    fmov.s fr2, @-r6
    fmov.s @r2, fr4
    add #-0x8, r2
    fmov.s @r10, fr2
    add #-0x8, r10
    fsub fr2, fr4
    fsub fr3, fr5
    fmac fr0, fr4, fr2
    fmac fr0, fr5, fr3
    fmov.s fr2, @-r6
    fschg
    fmov.s fr1, @-r6
    fmov.s @r2, fr5
    add #-0x4, r2
    fmov.s @r10, fr3
    add #-0x4, r10
    fmov.s @r2, fr4
    fmov.s @r10, fr2
    fsub fr3, fr5
    fsub fr2, fr4
    mov #-0x10, r2
    fmac fr0, fr5, fr3
    shll16 r2
    fmac fr0, fr4, fr2
    shll8 r2
    fmul fr1, fr3
    add #0x20, r5
    fmul fr1, fr2
    fmov.s fr3, @-r6
    fmov.s fr2, @-r6
    bt .Lmt7_vertex_colours_block_0169
    mov r6, r2
.Lmt7_vertex_colours_block_0169:
    mov.l r2, @-r6
    pref @r6
    rts
    add #0x20, r6
.Lmt7_vertex_colours_block_0170:
    .long .Lmt7_vertex_colours_block_0102
.Lmt7_vertex_colours_block_0171:
    .long .Lmt7_vertex_colours_block_0103
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0172:
    mov.l r10, @-r15
    mov.l r11, @-r15
    sts.l pr, @-r15
    mov.l @r4+, r3
.Lmt7_vertex_colours_block_0173:
    bsr .Lmt7_vertex_colours_block_0181
    mov #0x0, r9
    mov r9, r0
    cmp/eq #0x0,r0
    bt .Lmt7_vertex_colours_block_0176
    cmp/eq #0x7,r0
    bt .Lmt7_vertex_colours_block_0177
    bsr .Lmt7_vertex_colours_block_0160
    mov #0x0, r11
    bsr .Lmt7_vertex_colours_block_0164
    nop
    bsr .Lmt7_vertex_colours_block_0160
    mov #0x1, r11
    bsr .Lmt7_vertex_colours_block_0164
    nop
    bsr .Lmt7_vertex_colours_block_0160
    mov #0x2, r11
    bsr .Lmt7_vertex_colours_block_0164
    nop
    mov r12, r0
    tst #0x1,r0
    bt/s .Lmt7_vertex_colours_block_0174
    shll2 r9
    bsr .Lmt7_vertex_colours_block_0165
    nop
.Lmt7_vertex_colours_block_0174:
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
.Lmt7_vertex_colours_block_0175:
    dt r3
    bf .Lmt7_vertex_colours_block_0173
    mov #0x1, r0
    lds.l @r15+,pr
    or r0, r12
    mov.l @r15+, r11
    rts
    mov.l @r15+, r10
.Lmt7_vertex_colours_block_0176:
    mov r3, r9
    bsr .Lmt7_vertex_colours_block_0160
    mov #0x3, r3
    bsr .Lmt7_vertex_colours_block_0162
    nop
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0162
    nop
    bsr .Lmt7_vertex_colours_block_0160
    nop
    bsr .Lmt7_vertex_colours_block_0162
    nop
    bra .Lmt7_vertex_colours_block_0175
    mov r9, r3
.Lmt7_vertex_colours_block_0177:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0178
    add #0x18, r4
.Lmt7_vertex_colours_block_0178:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0179
    add #0x18, r4
.Lmt7_vertex_colours_block_0179:
    mov.l @r4, r0
    tst #0x1,r0
    add #0x8, r4
    bt .Lmt7_vertex_colours_block_0180
    add #0x18, r4
.Lmt7_vertex_colours_block_0180:
    bra .Lmt7_vertex_colours_block_0175
    nop
    .word 0x0008
    .word 0xbeb6
    .word 0xe000
    .word 0x0008
    .word 0xbeb3
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbecf
    .word 0xe000
    .word 0x0018
    .word 0xbecc
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbea7
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbec3
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbe9f
    .word 0xe002
    .word 0x0018
    .word 0xbebc
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbe97
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbeb3
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbeaf
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbea9
    .word 0xe000
    .word 0x0008
    .word 0xbe86
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbea2
    .word 0xe000
    .word 0x0018
    .word 0xbe7f
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbe99
    .word 0xe000
    .word 0x0008
    .word 0xbe76
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbe92
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbe8c
    .word 0xe000
    .word 0x0008
    .word 0xbe89
    .word 0xe001
    .word 0x0018
    .word 0xbe66
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_vertex_colours_block_0181:
    mov r4, r1
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0182
    fmov.s @r4+, fr4
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr4
    fmov.s @r0+, fr5
    fmov.s @r0+, fr6
    bra .Lmt7_vertex_colours_block_0183
    fldi1 fr7
.Lmt7_vertex_colours_block_0182:
    fmov.s @r4+, fr5
    fmov.s @r4, fr6
    fldi1 fr7
    add #0x18, r4
.Lmt7_vertex_colours_block_0183:
    pref @r4
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    ftrv xmtrx, fv4
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0184
    fmov.s @r4+, fr0
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr0
    fmov.s @r0+, fr1
    fmov.s @r0+, fr2
    bra .Lmt7_vertex_colours_block_0185
    fldi1 fr3
.Lmt7_vertex_colours_block_0184:
    fmov.s @r4+, fr1
    fmov.s @r4, fr2
    fldi1 fr3
    add #0x18, r4
.Lmt7_vertex_colours_block_0185:
    add #0x20, r4
    pref @r4
    add #-0x20, r4
    ftrv xmtrx, fv0
    flds fr7, fpul
    mov.l @r4, r0
    tst #0x1,r0
    bf/s .Lmt7_vertex_colours_block_0186
    fmov.s @r4+, fr4
    mov.l @r4+, r0
    add r4, r0
    fmov.s @r0+, fr4
    fmov.s @r0+, fr5
    fmov.s @r0+, fr6
    bra .Lmt7_vertex_colours_block_0187
    fldi1 fr7
.Lmt7_vertex_colours_block_0186:
    fmov.s @r4+, fr5
    fmov.s @r4, fr6
    fldi1 fr7
.Lmt7_vertex_colours_block_0187:
    mov r1, r4
    sts fpul,r0
    ftrv xmtrx, fv4
    cmp/hi r0, r12
    flds fr3, fpul
    rotcl r9
    sts fpul,r0
    cmp/hi r0, r12
    flds fr7, fpul
    rotcl r9
    sts fpul,r0
    cmp/hi r0, r12
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
    .word 0x0000

#endif
