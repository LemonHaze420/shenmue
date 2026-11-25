#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

    .section .text.shenmue_dc_hand_skin_normals,"ax"
    .align 2
    .global _shenmue_dc_hand_skin_normals
    .type _shenmue_dc_hand_skin_normals,@function
_shenmue_dc_hand_skin_normals:
    mov.l r8, @-r15
    mov.l r9, @-r15
    mov.l r10, @-r15
    mov.l r11, @-r15
    mov.l r12, @-r15
    mov.l r13, @-r15
    mov.l r14, @-r15
    fmov.s fr12, @-r15
    fmov.s fr13, @-r15
    fmov.s fr14, @-r15
    fmov.s fr15, @-r15
    mov r5, r14
    mov.l @(0x4,r14), r13
    cmp/pl r13
    bt/s .Lhand_skin_normal_0
    nop
    mov #0x1, r1
    add #0x24, r4
    mov #-0x18, r0
    mov #-0x40, r2
    extu.b r2, r2
    bra .Lhand_skin_normal_1
    nop
    bra .Lhand_skin_normal_2
    nop
.Lhand_skin_normal_0:
    add #0x24, r4
    mov #-0x18, r0
    mov #0x1, r1
    mov #-0x40, r2
    extu.b r2, r2
    mov #0x2, r7
    tst r7, r13
    bt .Lhand_skin_normal_3
    mov #0x1, r7
    tst r7, r13
    bt .Lhand_skin_normal_4
    fmov.s @r4+, fr0
    fmov.s @r4+, fr1
    fmov.s @r4+, fr2
    fsub fr3, fr3
    mov.l @(r0,r4), r8
    fipr fv0, fv0
    fsrra fr3
    fmul fr3, fr2
    or r1, r8
    fmul fr3, fr1
    mov.l r8, @(r0,r4)
    fmul fr3, fr0
    fmov.s fr2, @-r4
    fmov.s fr1, @-r4
    fmov.s fr0, @-r4
    add #0x30, r4
.Lhand_skin_normal_4:
    fmov.s @r4+, fr0
    fmov.s @r4+, fr1
    fmov.s @r4+, fr2
    fsub fr3, fr3
    mov.l @(r0,r4), r8
    fipr fv0, fv0
    fsrra fr3
    fmul fr3, fr2
    or r1, r8
    fmul fr3, fr1
    mov.l r8, @(r0,r4)
    fmul fr3, fr0
    fmov.s fr2, @-r4
    fmov.s fr1, @-r4
    fmov.s fr0, @-r4
    add #0x30, r4
.Lhand_skin_normal_3:
    mov r4, r7
    fmov.s @r7+, fr12
    add #0x30, r4
    fmov.s @r7+, fr13
    fsub fr15, fr15
    fmov.s @r7+, fr14
    mov r4, r5
    fmov.s @r4+, fr0
    add #0x30, r5
    fmov.s @r4+, fr1
    fsub fr3, fr3
    fmov.s @r4+, fr2
    fipr fv12, fv12
    mov.l @(r0,r7), r11
    mov r5, r6
    fmov.s @r5+, fr4
    fsub fr7, fr7
    fmov.s @r5+, fr5
    fipr fv0, fv0
    fmov.s @r5+, fr6
    add #0x30, r6
    mov.l @(r0,r4), r8
    fsrra fr15
    mov.l @(r0,r5), r9
    mov r6, r12
    fmov.s @r6+, fr8
    or r1, r11
    fmov.s @r6+, fr9
    add #0x30, r12
    fmov.s @r6+, fr10
    fipr fv4, fv4
    mov.l r11, @(r0,r7)
    or r1, r8
    mov.l @(r0,r6), r10
    or r1, r9
    fldi0 fr11
    fmul fr15, fr14
    shlr2 r13
    fmul fr15, fr13
    fmul fr15, fr12
    fldi0 fr15
    fsrra fr3
    fmov.s fr14, @-r7
    fmov.s fr13, @-r7
    dt r13
    fmov.s fr12, @-r7
    mov r12, r7
    bra .Lhand_skin_normal_5
    fsrra fr7
    mov r4, r5
    fmov.s @r4+, fr0
    add #0x30, r5
    fmov.s @r4+, fr1
    fsub fr3, fr3
    fmov.s @r4+, fr2
    mov r5, r6
    fmov.s @r5+, fr4
    add #0x30, r6
    fmov.s @r5+, fr5
    fsub fr7, fr7
    fmov.s @r5+, fr6
    fipr fv0, fv0
    mov.l @(r0,r4), r8
    mov r6, r7
    fmov.s @r6+, fr8
    add #0x30, r7
    fmov.s @r6+, fr9
    fipr fv4, fv4
    mov.l @(r0,r5), r9
    shlr2 r13
    fmov.s @r6+, fr10
    fsrra fr3
    mov.l @(r0,r6), r10
    or r1, r8
    fldi0 fr11
    or r1, r9
    fldi0 fr15
    dt r13
    bra .Lhand_skin_normal_5
    fsrra fr7
.Lhand_skin_normal_6:
    fsrra fr15
    fmov.s @r4+, fr0
    or r1, r11
    fmov.s @r4+, fr1
    add r2, r5
    fmov.s @r4+, fr2
    nop
    mov.l @(r0,r4), r8
    fsub fr3, fr3
    fldi0 fr7
    fmul fr11, fr10
    fmov.s @r5+, fr4
    fmul fr11, fr9
    fmov.s @r5+, fr5
    fmul fr11, fr8
    fmov.s @r5+, fr6
    fipr fv0, fv0
    mov.l @(r0,r5), r9
    or r1, r8
    mov.l r10, @(r0,r6)
    fmul fr15, fr14
    fmov.s fr10, @-r6
    fmul fr15, fr13
    fmov.s fr9, @-r6
    fmul fr15, fr12
    fmov.s fr8, @-r6
    fipr fv4, fv4
    mov.l r11, @(r0,r7)
    fsrra fr3
    fmov.s fr14, @-r7
    add r2, r6
    fmov.s fr13, @-r7
    nop
    fmov.s fr12, @-r7
    fsrra fr7
    fmov.s @r6+, fr8
    or r1, r9
    fmov.s @r6+, fr9
    add r2, r7
    fmov.s @r6+, fr10
    dt r13
    mov.l @(r0,r6), r10
    fsub fr11, fr11
    fldi0 fr15
.Lhand_skin_normal_5:
    fmul fr3, fr2
    fmov.s @r7+, fr12
    fmul fr3, fr1
    fmov.s @r7+, fr13
    fmul fr3, fr0
    fmov.s @r7+, fr14
    fipr fv8, fv8
    mov.l @(r0,r7), r11
    or r1, r10
    mov.l r8, @(r0,r4)
    fmul fr7, fr6
    fmov.s fr2, @-r4
    fmul fr7, fr5
    fmov.s fr1, @-r4
    fmul fr7, fr4
    fmov.s fr0, @-r4
    fipr fv12, fv12
    mov.l r9, @(r0,r5)
    fsrra fr11
    fmov.s fr6, @-r5
    add r2, r4
    fmov.s fr5, @-r5
    bf/s .Lhand_skin_normal_6
    fmov.s fr4, @-r5
    fsrra fr15
    or r1, r11
    fmul fr11, fr10
    mov.l r10, @(r0,r6)
    fmul fr11, fr9
    mov.l r11, @(r0,r7)
    fmul fr11, fr8
    fmul fr15, fr14
    fmov.s fr10, @-r6
    fmul fr15, fr13
    fmov.s fr9, @-r6
    fmul fr15, fr12
    fmov.s fr8, @-r6
    fmov.s fr14, @-r7
    fmov.s fr13, @-r7
    fmov.s fr12, @-r7
.Lhand_skin_normal_1:
    add #0xc, r4
    mov.l @(0x8,r14), r13
    cmp/pl r13
    bf/s .Lhand_skin_normal_7
    add #0xc, r14
    mov #0x3, r7
    tst r7, r13
    bt .Lhand_skin_normal_8
.Lhand_skin_normal_9:
    mov.l @(r0,r4), r8
    or r1, r8
    mov.l r8, @(r0,r4)
    add #0x30, r4
    dt r13
    bt .Lhand_skin_normal_7
    tst r7, r13
    bf .Lhand_skin_normal_9
.Lhand_skin_normal_8:
    mov r4, r5
    sub r2, r5
    add #0x30, r5
    mov r5, r6
    add #0x30, r6
    mov r6, r7
    add #0x30, r7
    shlr2 r13
.Lhand_skin_normal_10:
    mov.l @(r0,r4), r8
    add r2, r5
    mov.l @(r0,r5), r9
    add r2, r6
    mov.l @(r0,r6), r10
    add r2, r7
    mov.l @(r0,r7), r11
    or r1, r8
    mov.l r8, @(r0,r4)
    or r1, r9
    mov.l r9, @(r0,r5)
    or r1, r10
    mov.l r10, @(r0,r6)
    or r1, r11
    mov.l r11, @(r0,r7)
    dt r13
    bf/s .Lhand_skin_normal_10
    add r2, r4
.Lhand_skin_normal_2:
.Lhand_skin_normal_7:
    mov r14, r0
    fmov.s @r15+, fr15
    fmov.s @r15+, fr14
    fmov.s @r15+, fr13
    fmov.s @r15+, fr12
    mov.l @r15+, r14
    mov.l @r15+, r13
    mov.l @r15+, r12
    mov.l @r15+, r11
    mov.l @r15+, r10
    mov.l @r15+, r9
    mov.l @r15+, r8
    rts
    nop
    .size _shenmue_dc_hand_skin_normals,.-_shenmue_dc_hand_skin_normals

#endif
