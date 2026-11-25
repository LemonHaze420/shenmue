#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !(SHENMUE_IS_DC && SHENMUE_IS_S2)

    .section .text.shenmue_dc_hand_joint_transform,"ax"
    .align 2
    .global _shenmue_dc_hand_joint_transform
    .type _shenmue_dc_hand_joint_transform,@function
// @verified - d
_shenmue_dc_hand_joint_transform:
    extu.w r5, r5
    fschg 
    mov #-0x8, r0
    lds r5,FPUL
    fsca FPUL, dr2
    extu.w r4, r4
    mov r15, r3
    and r0, r15
    mova .Lhand_joint_transform_constants, r0
    lds r4,FPUL
    fsca FPUL, dr8
    fldi0 fr7
    mov r0, r1
    fmov.s fr4, @-r0
    extu.w r6, r6
    fmov.s fr12, @-r15
    nop 
    lds r6,FPUL
    fsca FPUL, dr4
    fmov.s @r1, fr0
    nop 
    fmov.s fr14, @-r15
    nop 
    fmov.s @r1, fr10
    nop 
    fmov fr8, fr12
    nop 
    fmov.s fr6, @-r0
    fmul fr2, fr8
    fmov fr10, fr14
    fmul fr2, fr13
    fneg fr2
    fsub fr4, fr1
    flds fr5, FPUL
    fipr fv8, fv4
    fsts FPUL, fr0
    nop 
    fldi0 fr6
    nop 
    flds fr3, FPUL
    fipr fv0, fv8
    fsts FPUL, fr10
    fipr fv12, fv0
    fneg fr1
    fipr fv4, fv12
    fmov fr6, fr4
    nop 
    fsts FPUL, fr6
    fadd fr11, fr4
    fldi0 fr7
    fmul fr12, fr6
    fsts FPUL, fr12
    fmul fr9, fr10
    fmov fr2, fr8
    fmul fr12, fr0
    flds fr15, FPUL
    fmul fr12, fr1
    fsts FPUL, fr8
    ftrv xmtrx, fv4
    fldi0 fr3
    fneg fr9
    fldi0 fr11
    fmov.s @r0+, fr14
    ftrv xmtrx, fv0
    fmov.s @r0+, fr12
    fldi1 fr15
    ftrv xmtrx, fv8
    ftrv xmtrx, fv12
    fmov.s @r15+, fr15
    fmov.s @r15+, fr13
    mov r3, r15
    fschg 
    rts 
    frchg 
    .size _shenmue_dc_hand_joint_transform,.-_shenmue_dc_hand_joint_transform

    .balign 8
.Lhand_joint_transform_scratch:
    .space 16
.Lhand_joint_transform_constants:
    .space 8

#endif
