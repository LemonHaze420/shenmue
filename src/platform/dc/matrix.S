#include "shenmue.h"

#if SHENMUE_IS_DC

    .section .text.dc_matrix_identity,"ax"
    .global _shenmue_dc_matrix_identity
    .type _shenmue_dc_matrix_identity,@function
// @verified - d
_shenmue_dc_matrix_identity:
    stc.l sr,@-r15
    sts.l pr,@-r15
    mov.l r0,@-r15
    mov.l r4,@-r15
    mov.l r5,@-r15
    mov.l .Lunit_matrix,r0
    jsr @r0
    mov #0,r4
    fldi1 fr0
    fldi0 fr1
    fldi0 fr2
    fldi1 fr3
    mov.l @r15+,r5
    mov.l @r15+,r4
    mov.l @r15+,r0
    lds.l @r15+,pr
    ldc.l @r15+,sr
    rts
    nop
    .align 2
.Lunit_matrix: .long _njUnitMatrix
    .size _shenmue_dc_matrix_identity,.-_shenmue_dc_matrix_identity

    .section .bss.dc_matrix,"aw",@nobits
    .balign 32
    .space 16
    .global _shenmue_dc_current_matrix_stack
    .type _shenmue_dc_current_matrix_stack,@object
_shenmue_dc_current_matrix_stack:
    .space 1
    .size _shenmue_dc_current_matrix_stack,1
    .space 3
    .global _shenmue_dc_matrix_stack
    .type _shenmue_dc_matrix_stack,@object
_shenmue_dc_matrix_stack:
    .space 0x30
    .size _shenmue_dc_matrix_stack,0x30
    .global _shenmue_dc_temporary_matrix_stacks
    .type _shenmue_dc_temporary_matrix_stacks,@object
_shenmue_dc_temporary_matrix_stacks:
    .space 0x180
    .size _shenmue_dc_temporary_matrix_stacks,0x180

#endif
