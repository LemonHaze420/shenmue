#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

    .section .text.dc_mt7_matrix,"ax"


    .balign 4
    .global _shenmue_dc_mt7_multiply_projection_rows
    .type _shenmue_dc_mt7_multiply_projection_rows,@function
// @verified - d
_shenmue_dc_mt7_multiply_projection_rows:
    fmov.s fr15,@-r15
    fmov.s fr14,@-r15
    fmov.s fr13,@-r15
    fmov.s fr12,@-r15
    fschg
    fmov.s @r4+,fr0
    fmov.s @r4+,fr2
    fmov.s @r4+,fr4
    fmov.s @r4+,fr6
    ftrv xmtrx,fv0
    fmov.s @r4+,fr8
    fmov.s @r4+,fr10
    ftrv xmtrx,fv4
    fmov.s @r4+,fr12
    fmov.s @r4+,fr14
    ftrv xmtrx,fv8
    ftrv xmtrx,fv12
    fschg
    frchg
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    rts
    fmov.s @r15+,fr15
    .size _shenmue_dc_mt7_multiply_projection_rows,.-_shenmue_dc_mt7_multiply_projection_rows

#endif
