/* AM2 hand-written vector and matrix helpers that GameJam calls from game code. Straight-line SH-4 code with
   C linkage; each routine reproduces GameJam's instruction stream. */

    .text
    .little

    .section .text.am2Vector3DistanceSquared,"ax",@progbits
    .align 1
    .global _am2Vector3DistanceSquared
_am2Vector3DistanceSquared:
    fmov	@r4+,fr4
    fmov	@r5+,fr8
    fmov	@r4+,fr5
    fmov	@r5+,fr9
    fmov	@r4+,fr6
    fmov	@r5+,fr10
    fldi0	fr7
    fsub	fr8,fr4
    fldi0	fr11
    fsub	fr9,fr5
    fsub	fr10,fr6
    fipr	fv4,fv4
    rts
    fmov	fr7,fr0

    .section .text.am2NegativeSineFixed,"ax",@progbits
    .align 1
    .global _am2NegativeSineFixed
_am2NegativeSineFixed:
    lds	r4,fpul
    fsca	fpul,dr0
    rts
    fneg	fr0

    .section .text.am2NegativeCosineFixed,"ax",@progbits
    .align 1
    .global _am2NegativeCosineFixed
_am2NegativeCosineFixed:
    lds	r4,fpul
    fsca	fpul,dr0
    fmov	fr1,fr0
    rts
    fneg	fr0

    .section .text.am2StoreXmtrx,"ax",@progbits
    .align 1
    .global _am2StoreXmtrx
_am2StoreXmtrx:
    add	#64,r4
    fschg
    fmov	fr15,@-r4
    fmov	fr13,@-r4
    fmov	fr11,@-r4
    fmov	fr9,@-r4
    fmov	fr7,@-r4
    fmov	fr5,@-r4
    fmov	fr3,@-r4
    fmov	fr1,@-r4
    rts
    fschg

    .section .text.am2LoadXmtrx,"ax",@progbits
    .align 1
    .global _am2LoadXmtrx
_am2LoadXmtrx:
    frchg
    fmov	@r4+,fr0
    fmov	@r4+,fr1
    fmov	@r4+,fr2
    fmov	@r4+,fr3
    fmov	@r4+,fr4
    fmov	@r4+,fr5
    fmov	@r4+,fr6
    fmov	@r4+,fr7
    fmov	@r4+,fr8
    fmov	@r4+,fr9
    fmov	@r4+,fr10
    fmov	@r4+,fr11
    fmov	@r4+,fr12
    fmov	@r4+,fr13
    fmov	@r4+,fr14
    fmov	@r4+,fr15
    rts
    frchg

    .section .text.am2LoadXmtrxPairs,"ax",@progbits
    .align 1
    .global _am2LoadXmtrxPairs
_am2LoadXmtrxPairs:
    fschg
    fmov	@r4+,fr1
    fmov	@r4+,fr3
    fmov	@r4+,fr5
    fmov	@r4+,fr7
    fmov	@r4+,fr9
    fmov	@r4+,fr11
    fmov	@r4+,fr13
    fmov	@r4+,fr15
    rts
    fschg

    .section .text.hmtStoreXmtrx,"ax",@progbits
    .align 1
    .global _hmtStoreXmtrx
_hmtStoreXmtrx:
    add	#64,r4
    fschg
    fmov	fr15,@-r4
    fmov	fr13,@-r4
    fmov	fr11,@-r4
    fmov	fr9,@-r4
    fmov	fr7,@-r4
    fmov	fr5,@-r4
    fmov	fr3,@-r4
    fmov	fr1,@-r4
    rts
    fschg

    .section .text.hmtLoadXmtrxPairs,"ax",@progbits
    .align 1
    .global _hmtLoadXmtrxPairs
_hmtLoadXmtrxPairs:
    fschg
    fmov	@r4+,fr1
    fmov	@r4+,fr3
    fmov	@r4+,fr5
    fmov	@r4+,fr7
    fmov	@r4+,fr9
    fmov	@r4+,fr11
    fmov	@r4+,fr13
    fmov	@r4+,fr15
    rts
    fschg

    .section .text.am2ReadUnalignedLe32,"ax",@progbits
    .align 1
    .global _am2ReadUnalignedLe32
_am2ReadUnalignedLe32:
    add	r5,r4
    add	#-4,r4
    mov	#0,r1
    mov.b	@(3,r4),r0
    extu.b	r0,r1
    swap.b	r1,r1
    mov.b	@(2,r4),r0
    extu.b	r0,r0
    or	r0,r1
    swap.w	r1,r1
    mov.b	@(1,r4),r0
    extu.b	r0,r0
    or	r0,r1
    swap.b	r1,r1
    mov.b	@(0,r4),r0
    extu.b	r0,r0
    rts
    or	r1,r0

    .section .text.am2ScaleXmtrxRows,"ax",@progbits
    .align 1
    .global _am2ScaleXmtrxRows
_am2ScaleXmtrxRows:
    frchg
    flds	fr15,fpul
    fmov	@r4+,fr15
    fmul	fr15,fr0
    fmul	fr15,fr1
    fmul	fr15,fr2
    fmul	fr15,fr3
    fmov	@r4+,fr15
    fmul	fr15,fr4
    fmul	fr15,fr5
    fmul	fr15,fr6
    fmul	fr15,fr7
    fmov	@r4+,fr15
    fmul	fr15,fr8
    fmul	fr15,fr9
    fmul	fr15,fr10
    fmul	fr15,fr11
    fsts	fpul,fr15
    rts
    frchg

    .section .text.am2Vector3AddInPlace,"ax",@progbits
    .align 1
    .global _am2Vector3AddInPlace
_am2Vector3AddInPlace:
    fmov	@r4+,fr0
    mov	#-8,r0
    fmov	@r5+,fr4
    fmov	@r4+,fr1
    fmov	@r5+,fr5
    fadd	fr4,fr0
    fmov	@r4,fr2
    fmov	@r5,fr6
    fadd	fr5,fr1
    fmov	fr0,@(r0,r4)
    mov	#-4,r0
    fadd	fr6,fr2
    fmov	fr1,@(r0,r4)
    rts
    fmov	fr2,@r4

    .section .text.am2Vector3Add,"ax",@progbits
    .align 1
    .global _am2Vector3Add
_am2Vector3Add:
    fmov	@r5+,fr0
    fmov	@r6+,fr4
    fmov	@r5+,fr1
    fmov	@r6+,fr5
    fadd	fr4,fr0
    fmov	@r5,fr2
    fmov	@r6,fr6
    fadd	fr5,fr1
    fmov	fr0,@r4
    add	#4,r4
    fadd	fr6,fr2
    fmov	fr1,@r4
    add	#4,r4
    rts
    fmov	fr2,@r4

    .section .text.am2Vector3Combine,"ax",@progbits
    .align 1
    .global _am2Vector3Combine
_am2Vector3Combine:
    fmov	fr5,fr0
    fmov	@r4+,fr1
    mov	#-4,r0
    fmov	@r4+,fr2
    fmov	@r4,fr3
    fmul	fr4,fr1
    fmov	@r5+,fr5
    fmul	fr4,fr2
    fmov	@r5+,fr6
    fmul	fr4,fr3
    fmov	@r5,fr7
    fmac	fr0,fr5,fr1
    add	#4,r6
    fmac	fr0,fr6,fr2
    fmov	fr1,@(r0,r6)
    fmac	fr0,fr7,fr3
    fmov	fr2,@r6
    add	#4,r6
    rts
    fmov	fr3,@r6

    .section .text.am2Vector3Dot,"ax",@progbits
    .align 1
    .global _am2Vector3Dot
_am2Vector3Dot:
    fmov	@r4+,fr0
    fmov	@r4+,fr1
    fmov	@r4+,fr2
    fldi0	fr3
    fmov	@r5+,fr4
    fmov	@r5+,fr5
    fmov	@r5+,fr6
    fldi0	fr7
    fipr	fv0,fv4
    rts
    fmov	fr7,fr0

    .section .text.am2Vector3Distance,"ax",@progbits
    .align 1
    .global _am2Vector3Distance
_am2Vector3Distance:
    fmov	@r4+,fr0
    fmov	@r5+,fr4
    fmov	@r4+,fr1
    fmov	@r5+,fr5
    fsub	fr4,fr0
    fmov	@r4,fr2
    fmov	@r5,fr6
    fsub	fr5,fr1
    fldi0	fr3
    fsub	fr6,fr2
    fipr	fv0,fv0
    fmov	fr3,fr0
    rts
    fsqrt	fr0

    .section .text.am2Vector3Cross,"ax",@progbits
    .align 1
    .global _am2Vector3Cross
_am2Vector3Cross:
    fmov	@r4+,fr0
    fmov	@r4+,fr5
    fmov	@r4,fr6
    fmov	@r5+,fr8
    fmov	@r5+,fr9
    fmov	@r5,fr10
    add	#12,r6
    fmov	fr5,fr3
    fmul	fr8,fr3
    fmov	fr0,fr2
    fmul	fr10,fr2
    fmov	fr6,fr1
    fmul	fr9,fr1
    fneg	fr3
    fmac	fr0,fr9,fr3
    fneg	fr2
    fmov	fr6,fr0
    fmac	fr0,fr8,fr2
    fneg	fr1
    fmov	fr3,@-r6
    fmov	fr5,fr0
    fmac	fr0,fr10,fr1
    fmov	fr2,@-r6
    rts
    fmov	fr1,@-r6

    .section .text.am2Vector3CrossLength,"ax",@progbits
    .align 1
    .global _am2Vector3CrossLength
_am2Vector3CrossLength:
    fmov	@r4+,fr0
    fmov	@r4+,fr5
    fmov	@r4,fr6
    fmov	@r5+,fr8
    fmov	@r5+,fr9
    fmov	@r5,fr10
    fmov	fr5,fr3
    fmul	fr8,fr3
    fmov	fr0,fr2
    fmul	fr10,fr2
    fmov	fr6,fr1
    fmul	fr9,fr1
    fneg	fr3
    fmac	fr0,fr9,fr3
    fneg	fr2
    fmov	fr6,fr0
    fmac	fr0,fr8,fr2
    fneg	fr1
    fmov	fr5,fr0
    fmac	fr0,fr10,fr1
    fldi0	fr0
    fipr	fv0,fv0
    fmov	fr3,fr0
    rts
    fsqrt	fr0

    .section .text.am2Vector3Scale,"ax",@progbits
    .align 1
    .global _am2Vector3Scale
_am2Vector3Scale:
    fmov	@r5+,fr0
    mov	#-4,r0
    fmov	@r5+,fr1
    add	#4,r4
    fmov	@r5,fr2
    fmul	fr4,fr0
    fmul	fr4,fr1
    fmul	fr4,fr2
    fmov	fr0,@(r0,r4)
    fmov	fr1,@r4
    add	#4,r4
    rts
    fmov	fr2,@r4

    .section .text.am2Vector3Length,"ax",@progbits
    .align 1
    .global _am2Vector3Length
_am2Vector3Length:
    fmov	@r4+,fr0
    fmov	@r4+,fr1
    fmov	@r4+,fr2
    fldi0	fr3
    fipr	fv0,fv0
    fmov	fr3,fr0
    rts
    fsqrt	fr0

    .section .text.am2Vector3LengthSquared,"ax",@progbits
    .align 1
    .global _am2Vector3LengthSquared
_am2Vector3LengthSquared:
    fmov	@r4+,fr0
    fmov	@r4+,fr1
    fmov	@r4+,fr2
    fldi0	fr3
    fipr	fv0,fv0
    rts
    fmov	fr3,fr0

    .section .text.am2Vector3SubtractInPlace,"ax",@progbits
    .align 1
    .global _am2Vector3SubtractInPlace
_am2Vector3SubtractInPlace:
    fmov	@r4+,fr0
    mov	#-8,r0
    fmov	@r5+,fr4
    fmov	@r4+,fr1
    fmov	@r5+,fr5
    fsub	fr4,fr0
    fmov	@r4,fr2
    fmov	@r5,fr6
    fsub	fr5,fr1
    fmov	fr0,@(r0,r4)
    mov	#-4,r0
    fsub	fr6,fr2
    fmov	fr1,@(r0,r4)
    rts
    fmov	fr2,@r4

    .section .text.am2Vector3Subtract,"ax",@progbits
    .align 1
    .global _am2Vector3Subtract
_am2Vector3Subtract:
    fmov	@r5+,fr0
    fmov	@r6+,fr4
    fmov	@r5+,fr1
    fmov	@r6+,fr5
    fsub	fr4,fr0
    fmov	@r5,fr2
    fmov	@r6,fr6
    fsub	fr5,fr1
    fmov	fr0,@r4
    add	#4,r4
    fsub	fr6,fr2
    fmov	fr1,@r4
    add	#4,r4
    rts
    fmov	fr2,@r4

    .section .text.am2Vector3NormalizeInPlace,"ax",@progbits
    .align 1
    .global _am2Vector3NormalizeInPlace
_am2Vector3NormalizeInPlace:
    mov	r4,r5
    fmov	@r4+,fr4
    fmov	@r4+,fr5
    fmov	@r4+,fr6
    fldi0	fr7
    add	#12,r5
    fipr	fv4,fv4
    fmov	fr7,fr0
    fsqrt	fr0
    fsrra	fr7
    fmul	fr7,fr6
    fmul	fr7,fr5
    fmul	fr7,fr4
    fmov	fr6,@-r5
    fmov	fr5,@-r5
    rts
    fmov	fr4,@-r5

    .section .text.am2Vector3Normalize,"ax",@progbits
    .align 1
    .global _am2Vector3Normalize
_am2Vector3Normalize:
    fmov	@r4+,fr4
    fmov	@r4+,fr5
    fmov	@r4+,fr6
    fldi0	fr7
    add	#12,r5
    fipr	fv4,fv4
    fmov	fr7,fr0
    fsqrt	fr0
    fsrra	fr7
    fmul	fr7,fr6
    fmul	fr7,fr5
    fmul	fr7,fr4
    fmov	fr6,@-r5
    fmov	fr5,@-r5
    rts
    fmov	fr4,@-r5

    .section .text.am2Vector3NormalizeFast,"ax",@progbits
    .align 1
    .global _am2Vector3NormalizeFast
_am2Vector3NormalizeFast:
    fmov	@r4+,fr4
    fmov	@r4+,fr5
    fmov	@r4+,fr6
    fldi0	fr7
    fipr	fv4,fv4
    fsrra	fr7
    fmul	fr7,fr6
    fmul	fr7,fr5
    fmul	fr7,fr4
    fmov	fr6,@-r4
    fmov	fr5,@-r4
    rts
    fmov	fr4,@-r4

    .section .text.am2MultiplyPlaneProjection,"ax",@progbits
    .align 2
    .global _am2MultiplyPlaneProjection
_am2MultiplyPlaneProjection:
    fmov	fr12,@-r15
    fmov	@r5+,fr0
    fmov	fr13,@-r15
    fmov	@r4+,fr8
    fmov	fr14,@-r15
    fmov	@r5+,fr1
    fmov	fr15,@-r15
    fmov	@r4+,fr9
    fmov	@r5+,fr2
    fmov	@r4+,fr10
    fldi0	fr3
    fmov	@r4+,fr4
    fldi0	fr11
    fmov	@r4+,fr5
    fldi0	fr7
    fmov	@r4+,fr6
    fipr	fv0,fv4
    fldi1	fr3
    fdiv	fr7,fr3
    mova	.Lam2PlaneProjectionEnd,r0
    fipr	fv8,fv4
    fmov	fr0,fr12
    fmul	fr3,fr7
    fmov	fr1,fr13
    fmul	fr7,fr12
    fmov	fr2,fr14
    fmul	fr7,fr13
    fldi1	fr15
    fmul	fr7,fr14
    fschg
    fmov	fr14,@-r0
    fmov	fr12,@-r0
    fschg
    fneg	fr3
    fmov	fr6,fr7
    fldi1	fr11
    fmov	fr0,fr12
    fmul	fr3,fr7
    fmov	fr1,fr13
    fmul	fr7,fr12
    fmov	fr2,fr14
    fmul	fr7,fr13
    fldi0	fr15
    fmul	fr7,fr14
    fadd	fr11,fr14
    fschg
    fmov	fr14,@-r0
    fmov	fr12,@-r0
    fschg
    fmov	fr5,fr7
    fmul	fr3,fr7
    fmov	fr0,fr12
    fmov	fr1,fr13
    fmul	fr7,fr12
    fmov	fr2,fr14
    fmul	fr7,fr13
    fldi0	fr15
    fmul	fr7,fr14
    fadd	fr11,fr13
    fschg
    fmov	fr14,@-r0
    fmov	fr12,@-r0
    fschg
    fmov	fr4,fr7
    fmul	fr3,fr7
    fmov	fr0,fr12
    fmov	fr1,fr13
    fmul	fr7,fr12
    fmov	fr2,fr14
    fmul	fr7,fr13
    fldi0	fr15
    fmul	fr7,fr14
    fadd	fr11,fr12
    fschg
    fmov	fr12,fr0
    fmov	fr14,fr2
    fmov	@r0+,fr4
    fmov	@r0+,fr6
    fmov	@r0+,fr8
    fmov	@r0+,fr10
    fmov	@r0+,fr12
    fmov	@r0+,fr14
    fschg
    ftrv	xmtrx,fv0
    ftrv	xmtrx,fv4
    ftrv	xmtrx,fv8
    ftrv	xmtrx,fv12
    frchg
    add	#-12,r5
    fmov	@r5+,fr0
    fmov	@r5+,fr1
    fldi1	fr3
    fmov	@r5+,fr2
    ftrv	xmtrx,fv0
    fschg
    fmov	fr0,fr13
    fmov	fr2,fr15
    fschg
    fmov	@r15+,fr15
    fmov	@r15+,fr14
    fmov	@r15+,fr13
    rts
    fmov	@r15+,fr12
    .align 2
.Lam2PlaneProjectionMatrix:
    .space 64
.Lam2PlaneProjectionEnd:
